#pragma once

#include <util/threadpool/BuddingTaskDropper.h>
#include <scanner/detector/PulseTask.h>
#include <scanner/detector/PulseThreadPool.h>
#include <noise/RandomnessGenerator.h>
#include <noise/NoiseSource.h>


//class PulseTaskDropper; // TODO Remove or Restore ?
/**
 * @author Alberto M. Esmoris Pena
 * @version 1.0
 * @brief Class implementing a task dropper to deal with pulse tasks
 * @see TaskDropper
 * @see PulseThreadPool
 */
class PulseTaskDropper : public BuddingTaskDropper<
    PulseTaskDropper,
    PulseTask,
    PulseThreadPool,
    std::vector<std::vector<double>>&,
    RandomnessGenerator<double>&,
    RandomnessGenerator<double>&,
    NoiseSource<double>&
>{
public:
    // ***  CONSTRUCTION / DESTRUCTION  *** //
    // ************************************ //
    /**
     * @brief Default constructor for PulseTaskDropper
     * @param maxTasks Value to initialize maximum tasks limit
     * @see TaskDropper
     * @see TaskDropper::maxTasks
     */
    PulseTaskDropper(
        int const maxTasks=32,
        int const delta1=8,
        int const initDelta1=8,
        int const delta2=1,
        char const lastSign=0
    ) :
        BuddingTaskDropper<
            PulseTaskDropper,
            PulseTask,
            PulseThreadPool,
            std::vector<std::vector<double>>&,
            RandomnessGenerator<double>&,
            RandomnessGenerator<double>&,
            NoiseSource<double>&
        >(maxTasks, delta1, initDelta1, delta2, lastSign)
    {}
    virtual ~PulseTaskDropper() = default;

    // ***  TASK DROPPER METHODS *** //
    // ***************************** //
    using TaskDropper::drop; // To avoid override hides drop overloads
    using TaskDropper::tryDrop; // To avoid override hides tryDrop overloads
    /**
     * @brief Like TaskDropper::drop but dropping all pulse tasks through a
     *  pulse thread pool.
     * @param pool Pulse thread pool to be used for parallel execution
     * @see TaskDropper::drop
     * @see PulseThreadPool
     * @see FullWaveformPulseRunnable
     */
    inline void drop(PulseThreadPool &pool) override
    {pool.run_res_task(*this);}
    /**
     * @brief Like TaskDropper::tryDrop but dropping all pulse tasks through a
     *  pulse thread pool in a non-blocking way
     * @param pool Pulse thread pool to be used for parallel execution
     * @see TaskDropper::tryDrop
     */
    inline bool tryDrop(PulseThreadPool &pool) override
    {return pool.try_run_res_task(*this);}
};
